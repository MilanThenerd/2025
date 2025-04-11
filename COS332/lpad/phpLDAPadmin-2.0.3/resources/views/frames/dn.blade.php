@extends('layouts.dn')

@section('page_title')
	@include('fragment.dn.header',['o'=>($o ?? $o=$server->fetch($dn))])
@endsection

@section('page_actions')
	<div class="row">
		<div class="col">
			<div class="action-buttons float-end">
				<ul class="nav">
					@if(isset($page_actions) && $page_actions->contains('export'))
						<li>
							<span id="entry-export" data-bs-toggle="modal" data-bs-target="#page-modal">
								<button class="btn btn-outline-dark p-1 m-1" data-bs-toggle="tooltip" data-bs-placement="bottom" title="@lang('Export')"><i class="fas fa-fw fa-download fs-5"></i></button>
							</span>
						</li>
					@endif
					@if(isset($page_actions) && $page_actions->contains('copy'))
						<li>
							<button class="btn btn-outline-dark p-1 m-1" id="entry-copy-move" data-bs-toggle="tooltip" data-bs-placement="bottom" title="@lang('Copy/Move')" disabled><i class="fas fa-fw fa-copy fs-5"></i></button>
						</li>
					@endif
					@if((isset($page_actions) && $page_actions->contains('edit')) || old())
						<li>
							<button class="btn btn-outline-dark p-1 m-1" id="entry-edit" data-bs-toggle="tooltip" data-bs-placement="bottom" title="@lang('Edit Entry')"><i class="fas fa-fw fa-edit fs-5"></i></button>
						</li>
					@endif
					<!-- @todo Dont offer the delete button for an entry with children -->
					@if(isset($page_actions) && $page_actions->contains('delete'))
						<li>
							<span id="entry-delete" data-bs-toggle="modal" data-bs-target="#page-modal">
								<button class="btn btn-outline-danger p-1 m-1" data-bs-custom-class="custom-tooltip-danger" data-bs-toggle="tooltip" data-bs-placement="bottom" title="@lang('Delete Entry')"><i class="fas fa-fw fa-trash-can fs-5"></i></button>
							</span>
						</li>
					@endif
				</ul>
			</div>
		</div>
	</div>
@endsection

@section('main-content')
	<x-note/>
	<x-updated/>
	<x-error/>

	<div class="main-card mb-3 card">
		<div class="card-body">
			<div class="card-header-tabs">
				<ul class="nav nav-tabs">
					<li class="nav-item"><a data-bs-toggle="tab" href="#attributes" class="nav-link active">@lang('Attributes')</a></li>
					<li class="nav-item"><a data-bs-toggle="tab" href="#internal" class="nav-link">@lang('Internal')</a></li>
					@env(['local'])
						<li class="nav-item"><a data-bs-toggle="tab" href="#debug" class="nav-link">@lang('Debug')</a></li>
					@endenv
				</ul>

				<div class="tab-content">
					<!-- All Attributes -->
					<div class="tab-pane active" id="attributes" role="tabpanel">
						<form id="dn-edit" method="POST" class="needs-validation" action="{{ url('entry/update/pending') }}" novalidate readonly>
							@csrf

							<input type="hidden" name="dn" value="">

							@foreach ($o->getVisibleAttributes() as $ao)
								<x-attribute-type :edit="true" :o="$ao"/>
							@endforeach

							@include('fragment.dn.add_attr')
						</form>

						<div class="row d-none pt-3">
							<div class="col-12 offset-sm-2 col-sm-4 col-lg-2">
								<x-form.reset form="dn-edit"/>
								<x-form.submit action="Update" form="dn-edit"/>
							</div>
						</div>
					</div>

					<!-- Internal Attributes -->
					<div class="tab-pane" id="internal" role="tabpanel">
						@foreach ($o->getInternalAttributes() as $ao)
							<x-attribute-type :o="$ao"/>
						@endforeach
					</div>

					<!-- Debug -->
					<div class="tab-pane" id="debug" role="tabpanel">
						<div class="row">
							<div class="col-4">
								@dump($o)
							</div>
							<div class="col-4">
								@dump($o->getAttributes())
							</div>
							<div class="col-4">
								@dump(['available'=>$o->getAvailableAttributes()->pluck('name'),'missing'=>$o->getMissingAttributes()->pluck('name')])
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
@endsection

@section('page-modals')
	<!-- Frame Modals -->
	<div class="modal fade" id="page-modal" tabindex="-1" aria-labelledby="label" aria-hidden="true">
		<div class="modal-dialog modal-lg modal-fullscreen-lg-down">
			<div class="modal-content"></div>
		</div>
	</div>
@endsection

@section('page-scripts')
	<script type="text/javascript">
		var dn = '{{ $o->getDNSecure() }}';
		var oc = {!! $o->getObject('objectclass')->values !!};

		function editmode() {
			$('#dn-edit input[name="dn"]').val(dn);

			$('form#dn-edit').attr('readonly',false);
			$('button[id=entry-edit]')
				.removeClass('btn-outline-dark')
				.addClass('btn-dark')
				.addClass('opacity-100')
				.attr('disabled',true);

			// Find all input items and turn off readonly
			$('input.form-control').each(function() {
				// Except for objectClass - @todo show an "X" instead
				if ($(this)[0].name.match(/^objectclass/))
					return;

				$(this).attr('readonly',false);
			});

			// Our password type
			$('attribute#userPassword .form-select').each(function() {
				$(this).prop('disabled',false);
			})

			$('.row.d-none').removeClass('d-none');
			$('span.addable.d-none').removeClass('d-none');
			$('span.deletable.d-none').removeClass('d-none');

			@if($o->getMissingAttributes()->count())
				$('#newattr-select.d-none').removeClass('d-none');
			@endif
		}

		$(document).ready(function() {
			$('#newattr').on('change',function(item) {
				$.ajax({
					type: 'POST',
					beforeSend: function() {},
					success: function(data) {
						$('#newattrs').append(data);
					},
					error: function(e) {
						if (e.status !== 412)
							alert('That didnt work? Please try again....');
					},
					url: '{{ url('entry/attr/add') }}/'+item.target.value,
					data: {
						objectclasses: oc,
					},
					cache: false
				});

				// Remove the option from the list
				$(this).find('[value="'+item.target.value+'"]').remove()

				// If there are no more options
				if ($(this).find("option").length === 1)
					$('#newattr-select').remove();
			});

			$('#page-modal').on('shown.bs.modal',function(item) {
				var that = $(this).find('.modal-content');

				switch ($(item.relatedTarget).attr('id')) {
					case 'entry-delete':
						$.ajax({
							method: 'GET',
							url: '{{ url('modal/delete') }}/'+dn,
							dataType: 'html',
							cache: false,
							beforeSend: function() {
								that.empty().append('<span class="p-3"><i class="fas fa-3x fa-spinner fa-pulse"></i></span>');
							},
							success: function(data) {
								that.empty().html(data);
							},
							error: function(e) {
								if (e.status !== 412)
									alert('That didnt work? Please try again....');
							},
						})
						break;

					case 'entry-export':
						$.ajax({
							method: 'GET',
							url: '{{ url('modal/export') }}/'+dn,
							dataType: 'html',
							cache: false,
							beforeSend: function() {
								that.empty().append('<span class="p-3"><i class="fas fa-3x fa-spinner fa-pulse"></i></span>');
							},
							success: function(data) {
								that.empty().html(data);

								that = $('#entry_export');

								$.ajax({
									method: 'GET',
									url: '{{ url('entry/export') }}/'+dn,
									cache: false,
									beforeSend: function() {
										that.empty().append('<span class="p-3"><i class="fas fa-3x fa-spinner fa-pulse"></i></span>');
									},
									success: function(data) {
										that.empty().append(data);
									},
									error: function(e) {
										if (e.status !== 412)
											alert('That didnt work? Please try again....');
									},
								})
							},
							error: function(e) {
								if (e.status !== 412)
									alert('That didnt work? Please try again....');
							},
						})
						break;

					case 'entry-userpassword-check':
						$.ajax({
							method: 'GET',
							url: '{{ url('modal/userpassword-check') }}/'+dn,
							dataType: 'html',
							cache: false,
							beforeSend: function() {
								that.empty().append('<span class="p-3"><i class="fas fa-3x fa-spinner fa-pulse"></i></span>');
							},
							success: function(data) {
								that.empty().html(data);
							},
							error: function(e) {
								if (e.status !== 412)
									alert('That didnt work? Please try again....');
							},
						})
						break;

					default:
						console.log('No action for button:'+$(item.relatedTarget).attr('id'));
				}
			});

			@if(old())
				editmode();
			@endif
		});
	</script>
@append